/**
 * Sentra Browser Controller — Playwright-based browser automation service
 *
 * Communicates with the C++ Scenario Engine via WebSocket JSON-RPC.
 * Provides browser control, screenshot capture, DOM access, and accessibility tree.
 *
 * Protocol: WebSocket JSON-RPC 2.0
 * Default port: 9515
 */

const { chromium } = require('playwright');
const WebSocket = require('ws');

class BrowserController {
    constructor(port = 9515) {
        this.port = port;
        this.browser = null;
        this.page = null;
        this.wss = null;
    }

    async start() {
        this.wss = new WebSocket.Server({ port: this.port });
        console.log(`[Sentra] Browser Controller listening on ws://localhost:${this.port}`);

        this.wss.on('connection', (ws) => {
            console.log('[Sentra] Engine connected');
            ws.on('message', async (data) => {
                try {
                    const request = JSON.parse(data);
                    const response = await this.handleRequest(request);
                    ws.send(JSON.stringify(response));
                } catch (error) {
                    ws.send(JSON.stringify({
                        jsonrpc: '2.0',
                        id: null,
                        error: { code: -32700, message: error.message }
                    }));
                }
            });

            ws.on('close', () => {
                console.log('[Sentra] Engine disconnected');
            });
        });
    }

    async handleRequest(request) {
        const { id, method, params } = request;
        const startTime = Date.now();

        try {
            let result;
            switch (method) {
                case 'launch':
                    result = await this.launch(params);
                    break;
                case 'navigate':
                    result = await this.navigate(params);
                    break;
                case 'click':
                    result = await this.click(params);
                    break;
                case 'type':
                    result = await this.typeText(params);
                    break;
                case 'screenshot':
                    result = await this.screenshot(params);
                    break;
                case 'getDom':
                    result = await this.getDom(params);
                    break;
                case 'getAccessibilityTree':
                    result = await this.getAccessibilityTree(params);
                    break;
                case 'waitFor':
                    result = await this.waitFor(params);
                    break;
                case 'evaluate':
                    result = await this.evaluate(params);
                    break;
                case 'close':
                    result = await this.closeBrowser();
                    break;
                default:
                    throw new Error(`Unknown method: ${method}`);
            }

            return {
                jsonrpc: '2.0',
                id,
                result: {
                    success: true,
                    data: result,
                    durationMs: Date.now() - startTime
                }
            };
        } catch (error) {
            return {
                jsonrpc: '2.0',
                id,
                result: {
                    success: false,
                    error: error.message,
                    durationMs: Date.now() - startTime
                }
            };
        }
    }

    async launch(params = {}) {
        const { headless = true, viewportWidth = 1920, viewportHeight = 1080 } = params;

        this.browser = await chromium.launch({ headless });
        const context = await this.browser.newContext({
            viewport: { width: viewportWidth, height: viewportHeight }
        });
        this.page = await context.newPage();

        return { status: 'launched', headless, viewport: { width: viewportWidth, height: viewportHeight } };
    }

    async navigate(params) {
        const { url, waitUntil = 'domcontentloaded' } = params;
        await this.page.goto(url, { waitUntil });
        return { url: this.page.url(), title: await this.page.title() };
    }

    async click(params) {
        const { selector, button = 'left', timeout = 5000 } = params;
        await this.page.click(selector, { button, timeout });
        return { clicked: selector };
    }

    async typeText(params) {
        const { selector, text, clearBefore = false, timeout = 5000 } = params;
        if (clearBefore) {
            await this.page.fill(selector, '');
        }
        await this.page.type(selector, text, { timeout });
        return { typed: text, selector };
    }

    async screenshot(params = {}) {
        const { fullPage = false, path, element } = params;

        let options = { fullPage };
        if (path) options.path = path;

        let buffer;
        if (element) {
            const el = await this.page.$(element);
            buffer = await el.screenshot(options);
        } else {
            buffer = await this.page.screenshot(options);
        }

        return {
            data: buffer.toString('base64'),
            encoding: 'base64',
            mimeType: 'image/png'
        };
    }

    async getDom(params = {}) {
        const dom = await this.page.evaluate(() => {
            return document.documentElement.outerHTML;
        });
        return { dom, url: this.page.url() };
    }

    async getAccessibilityTree(params = {}) {
        const snapshot = await this.page.accessibility.snapshot();
        return { tree: snapshot, url: this.page.url() };
    }

    async waitFor(params) {
        const { condition, timeout = 5000 } = params;

        if (condition.startsWith('selector:')) {
            const selector = condition.slice('selector:'.length);
            await this.page.waitForSelector(selector, { timeout });
        } else if (condition.startsWith('navigation')) {
            await this.page.waitForNavigation({ timeout });
        } else if (condition.startsWith('timeout:')) {
            const ms = parseInt(condition.slice('timeout:'.length));
            await this.page.waitForTimeout(ms);
        } else {
            await this.page.waitForFunction(condition, { timeout });
        }

        return { waited: condition };
    }

    async evaluate(params) {
        const { expression } = params;
        const result = await this.page.evaluate(expression);
        return { result };
    }

    async closeBrowser() {
        if (this.browser) {
            await this.browser.close();
            this.browser = null;
            this.page = null;
        }
        return { status: 'closed' };
    }

    async stop() {
        await this.closeBrowser();
        if (this.wss) {
            this.wss.close();
        }
    }
}

// Entry point
if (require.main === module) {
    const port = parseInt(process.env.SENTRA_BROWSER_PORT || '9515');
    const controller = new BrowserController(port);
    controller.start();

    process.on('SIGINT', async () => {
        await controller.stop();
        process.exit(0);
    });

    process.on('SIGTERM', async () => {
        await controller.stop();
        process.exit(0);
    });
}

module.exports = { BrowserController };
