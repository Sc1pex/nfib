defmodule NfibServerWeb.RunnerAPIController do
  use NfibServerWeb, :controller

  alias NfibServer.RunnerAPI

  def add(conn, req) do
    case RunnerAPI.add_runner_with_impls(req) do
      {:ok, _} ->
        json(conn, %{status: "ok"})

      {:error, :db_error} ->
        conn |> put_status(:internal_server_error) |> json(%{error: "Internal server error"})

      {:error, reason} ->
        conn |> put_status(:bad_request) |> json(%{error: reason})
    end
  end

  def remove(conn, req) do
    IO.inspect(req)
    json(conn, %{status: "ok"})
  end

  def debug_list(conn, req) do
    IO.inspect(req)
    json(conn, %{status: "ok"})
  end
end
