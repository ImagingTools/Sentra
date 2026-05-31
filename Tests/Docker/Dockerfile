FROM node:18-slim AS browser-controller

WORKDIR /app/browser
COPY Tools/Browser/package.json Tools/Browser/index.js ./
RUN npm ci --production
RUN npx playwright install chromium --with-deps

FROM ubuntu:22.04 AS sentra-engine

RUN apt-get update && apt-get install -y \
    libqt6core6 \
    libqt6network6 \
    libqt6websockets6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=browser-controller /app/browser /app/browser
COPY build/sentra-cli /app/sentra-cli

# Install Node.js for browser controller
RUN apt-get update && apt-get install -y curl \
    && curl -fsSL https://deb.nodesource.com/setup_18.x | bash - \
    && apt-get install -y nodejs \
    && rm -rf /var/lib/apt/lists/*

ENV SENTRA_BROWSER_PORT=9515
ENV SENTRA_HEADLESS=true

EXPOSE 9515
EXPOSE 4000

ENTRYPOINT ["/app/sentra-cli"]
CMD ["run", "--help"]
