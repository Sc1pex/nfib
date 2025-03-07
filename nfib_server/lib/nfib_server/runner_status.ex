defmodule NfibServer.RunnerStatus do
  alias NfibServer.Repo
  alias NfibServer.RunnerAPI
  use GenServer

  @update_interval :timer.seconds(10)

  def start_link(_) do
    GenServer.start_link(__MODULE__, nil, name: __MODULE__)
  end

  def update_status do
    GenServer.cast(__MODULE__, :update)
  end

  def get_status do
    GenServer.call(__MODULE__, :get_status, :infinity)
  end

  @impl true
  def init(nil) do
    state = runners_status()

    schedule_update()
    {:ok, state}
  end

  @impl true
  def handle_cast(:update, old_state) do
    state = runners_status()
    diff = state_diff(old_state, state)

    if MapSet.size(diff) > 0 do
      Phoenix.PubSub.broadcast(
        NfibServer.PubSub,
        "runners_status",
        {:runners_status_change, state}
      )
    end

    {:noreply, state}
  end

  @impl true
  def handle_call(:get_status, _from, state) do
    {:reply, state, state}
  end

  @impl true
  def handle_info(:periodic_update, old_state) do
    return = handle_cast(:update, old_state)
    schedule_update()

    return
  end

  defp schedule_update() do
    Process.send_after(self(), :periodic_update, @update_interval)
  end

  defp runners_status() do
    import Ecto.Query
    runners = Repo.all(from r in RunnerAPI.Runner, select: r.address)

    Map.new(runners, fn address ->
      case HTTPoison.get("#{address}/health", [], recv_timeout: 1000) do
        {:ok, %HTTPoison.Response{status_code: 200, body: "alive"}} -> {address, :online}
        _ -> {address, :offline}
      end
    end)
  end

  defp state_diff(old_state, new_state) do
    MapSet.difference(MapSet.new(new_state), MapSet.new(old_state))
  end
end
