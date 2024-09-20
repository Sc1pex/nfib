import './app.css'
import App from './App.svelte'
import { startReloader } from './livereload'

const app = new App({
  target: document.getElementById('app')!,
})

startReloader()

export default app
