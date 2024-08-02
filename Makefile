.ONESHELL: 

all: run

setup:
	@meson setup out

build:
	@cd out/
	@meson compile

run: build
	@./out/nfibb
