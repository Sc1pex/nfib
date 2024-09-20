#!/bin/sh

sudo chown -R vscode:vscode /home/vscode/.local
go install github.com/air-verse/air@latest
nvim --headless +"MasonInstall clangd gopls typescript-language-server tailwindcss-language-server svelte-language-server" +q
