defmodule NfibServerWeb.Router do
  use NfibServerWeb, :router

  pipeline :browser do
    plug :accepts, ["html"]
    plug :fetch_session
    plug :fetch_live_flash
    plug :put_root_layout, html: {NfibServerWeb.Layouts, :root}
    plug :protect_from_forgery
    plug :put_secure_browser_headers
  end

  pipeline :api do
    plug :accepts, ["json"]
  end

  scope "/", NfibServerWeb do
    pipe_through :browser

    get "/", HomeController, :index
  end

  scope "/runnerapi", NfibServerWeb do
    pipe_through :api

    post "/add", RunnerAPIController, :add
    post "/delete", RunnerAPIController, :delete

    get "/debugList", RunnerAPIController, :debug_list
  end
end
