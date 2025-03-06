defmodule NfibServer.RunnerStatus do
  alias NfibServer.Repo
  alias NfibServer.RunnerAPI
  use GenServer

  @update_interval :timer.seconds(10)

  def start_link(_) do
    GenServer.start_link(__MODULE__, nil, name: __MODULE__)
  end

  def update_status do
    GenServer.call(__MODULE__, :update)
  end

  def get_status do
    GenServer.call(__MODULE__, :get_status)
  end

  @impl true
  def init(nil) do
    state = runners_status()

    schedule_update()
    {:ok, state}
  end

  @impl true
  def handle_call(:update, _from, old_state) do
    handle_info(:update, old_state)
  end

  @impl true
  def handle_call(:get_status, _from, state) do
    {:reply, state, state}
  end

  @impl true
  def handle_info(:update, old_state) do
    state = runners_status()
    diff = state_diff(old_state, state)

    if map_size(diff) > 0 do
      Phoenix.PubSub.broadcast(
        NfibServer.PubSub,
        "runners_status",
        {:runners_status_change, state}
      )
    end

    schedule_update()
    {:noreply, state}
  end

  defp schedule_update() do
    Process.send_after(self(), :update, @update_interval)
  end

  defp runners_status() do
    import Ecto.Query
    runners = Repo.all(from r in RunnerAPI.Runner, select: r.address)

    Map.new(runners, fn address ->
      case HTTPoison.get("#{address}/health") do
        {:ok, %HTTPoison.Response{status_code: 200, body: "alive"}} -> {address, :online}
        _ -> {address, :offline}
      end
    end)
    |> IO.inspect()
  end

  defp state_diff(old_state, new_state) do
    MapSet.difference(MapSet.new(old_state), MapSet.new(new_state))
  end
end
