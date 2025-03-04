defmodule NfibServerWeb.HomeController do
  use NfibServerWeb, :controller

  def runners(conn, _params) do
    conn
    |> assign(:runners, NfibServer.RunnerAPI.runners())
    |> render(:runners)
  end
end
