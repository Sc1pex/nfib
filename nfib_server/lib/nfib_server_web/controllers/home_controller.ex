defmodule NfibServerWeb.HomeController do
  use NfibServerWeb, :controller

  def index(conn, _params) do
    render(conn, :index)
  end
end
