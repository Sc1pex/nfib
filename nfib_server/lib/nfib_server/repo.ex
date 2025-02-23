defmodule NfibServer.Repo do
  use Ecto.Repo,
    otp_app: :nfib_server,
    adapter: Ecto.Adapters.SQLite3
end
