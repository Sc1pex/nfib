defmodule NfibServerWeb.RunnersLive do
  use NfibServerWeb, :live_view

  def render(assigns) do
    ~H"""
    <h1 class="text-3xl mb-12">
      Registered runners
    </h1>

    <%= for {runner_name, data} <- @runners do %>
      <div class="collapse collapse-arrow border border-base-300 bg-base-200 my-4">
        <input type="checkbox" />
        <div class="collapse-title text-xl font-medium">{runner_name}</div>
        <div class="collapse-content text-sm relative">
          <p class="text-base-content/40">Address: {data[:address]}</p>
          <p class="mt-2 text-base-content/70">Impls:</p>
          <ul class="list-disc list-inside mt-2 text-sm">
            <%= for impl_name <- data[:impls] do %>
              <li>{impl_name}</li>
            <% end %>
          </ul>

          <button
            phx-click="delete"
            phx-value-runner_name={runner_name}
            class="btn btn-sm btn-ghost btn-error mt-4 absolute bottom-4 right-4"
          >
            <.icon name="hero-trash" class="h-5 w-5" />
          </button>
        </div>
      </div>
    <% end %>
    """
  end

  def mount(_params, _session, socket) do
    Phoenix.PubSub.subscribe(NfibServer.PubSub, "runners")
    {:ok, assign(socket, runners: NfibServer.RunnerAPI.runners())}
  end

  def handle_info(msg, socket) do
    case msg do
      :runners_update ->
        {:noreply, assign(socket, runners: NfibServer.RunnerAPI.runners())}

      _ ->
        {:noreply, socket}
    end
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
