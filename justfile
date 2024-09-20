watch-ui:
    cd ui; bun run build --watch

watch-server:
    cd server; air

watch:
    just watch-ui & just watch-server 
