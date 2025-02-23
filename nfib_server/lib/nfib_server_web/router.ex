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

  # Other scopes may use custom stacks.
  # scope "/api", NfibServerWeb do
  #   pipe_through :api
  # end
end
