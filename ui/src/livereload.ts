export function startReloader() {
    const socket_url = 'ws://' + location.host + '/livereload'
    let ws = new WebSocket(socket_url)
    ws.addEventListener("close", () => {
        const attempt_timeout = 100
        let attempts_remaning = 20

        const tryReconnect = () => {
            attempts_remaning -= 1
            if (attempts_remaning < 0) {
                console.error("Could not reconnect to server")
                return;
            }

            ws = new WebSocket(socket_url)
            ws.addEventListener("open", () => {
                location.reload()
            })
            ws.addEventListener("error", () => {
                setTimeout(tryReconnect, attempt_timeout)
            })
        }

        tryReconnect()
    })
}
