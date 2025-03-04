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
        <div class="collapse-content text-sm ">
          <p class="text-base-content/40">Address: {data[:address]}</p>
          <p class="mt-2 text-base-content/70">Impls:</p>
          <ul class="list-disc list-inside mt-2 text-sm">
            <%= for impl_name <- data[:impls] do %>
              <li>{impl_name}</li>
            <% end %>
          </ul>
        </div>
      </div>
    <% end %>
    """
  end

  def mount(_params, _session, socket) do
    {:ok, assign(socket, runners: NfibServer.RunnerAPI.runners())}
  end
end
