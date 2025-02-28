defmodule NfibServerWeb.RunnerAPIController do
  use NfibServerWeb, :json

  alias NfibServer.RunnerAPI

  json_handlers(
    add: &RunnerAPI.add_runner_with_impls/1,
    delete: &RunnerAPI.remove_runner/1
  )
end
