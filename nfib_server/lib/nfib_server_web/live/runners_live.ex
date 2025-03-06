defmodule NfibServerWeb.RunnersLive do
  use NfibServerWeb, :live_view
  import NfibServerWeb.IconComponents

  def mount(_params, _session, socket) do
    Phoenix.PubSub.subscribe(NfibServer.PubSub, "runners")
    Phoenix.PubSub.subscribe(NfibServer.PubSub, "runners_status")

    {:ok,
     socket
     |> assign(runners: NfibServer.RunnerAPI.runners())
     |> assign(runners_status: NfibServer.RunnerStatus.get_status())}
  end

  def handle_info(:runners_update, socket) do
    {:noreply, assign(socket, runners: NfibServer.RunnerAPI.runners())}
  end

  def handle_info({:runners_status_change, new_status}, socket) do
    {:noreply, assign(socket, runners_status: new_status)}
  end

  def handle_event("delete", %{"runner_name" => runner_name}, socket) do
    with {:ok, runner} <- NfibServer.RunnerAPI.runner_info(runner_name),
         {:ok, _} <-
           NfibServer.RunnerAPI.remove_runner(%{
             "runner" => %{
               "name" => runner[:name],
               "address" => runner[:address]
             }
           }) do
      {:noreply, assign(socket, runners: NfibServer.RunnerAPI.runners())}
    else
      {:error, reason} ->
        IO.inspect(reason)
        {:noreply, socket}
    end
  end
end
