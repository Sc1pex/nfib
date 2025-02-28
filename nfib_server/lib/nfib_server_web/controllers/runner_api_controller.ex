defmodule NfibServerWeb.RunnerAPIController do
  use NfibServerWeb, :json

  alias NfibServer.RunnerAPI

  json_handlers(
    add: &RunnerAPI.add_runner_with_impls/1,
    delete: &RunnerAPI.remove_runner/1
    # debug_list: &RunnerAPI.debug_list/1
  )
end
