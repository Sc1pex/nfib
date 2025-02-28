defmodule NfibServerWeb.JsonApiMacros do
  defmacro json_handler(name, func) do
    quote do
      def unquote(name)(conn, req) do
        case unquote(func).(req) do
          {:ok, _} ->
            json(conn, %{status: "ok"})

          {:error, :db_error} ->
            conn |> put_status(:internal_server_error) |> json(%{error: "Internal server error"})

          {:error, reason} ->
            conn |> put_status(:bad_request) |> json(%{error: reason})
        end
      end
    end
  end

  defmacro json_handlers(handlers) do
    Enum.map(handlers, fn {name, func} ->
      quote do
        json_handler(unquote(name), unquote(func))
      end
    end)
  end
end
