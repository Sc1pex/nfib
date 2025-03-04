defmodule NfibServer.RunnerAPI do
  require Logger

  alias NfibServer.Repo
  alias NfibServer.RunnerAPI.{Runner, Impl}

  def runners() do
    import Ecto.Query

    data =
      Repo.all(
        from r in Runner,
          join: i in Impl,
          on: i.runner_id == r.id,
          select: {r.name, r.address, i.name}
      )

    Enum.reduce(data, %{}, fn {name, addr, impl}, acc ->
      case acc[name] do
        nil ->
          Map.put(acc, name, %{address: addr, impls: [impl]})

        %{address: addr, impls: impls} ->
          Map.put(acc, name, %{address: addr, impls: [impl | impls]})
      end
    end)
  end

  def add_runner_with_impls(%{
        "runner" => %{"name" => r_name, "address" => r_addr} = runner,
        "impls" => impls
      })
      when is_list(impls) and is_binary(r_name) and is_binary(r_addr) do
    with {:ok, _} <- validate_impls(impls),
         {:ok, runner_id} <- add_runner(runner) do
      add_impl_result =
        Enum.reduce_while(impls, nil, fn impl_name, acc ->
          case add_impl(impl_name, runner_id) do
            {:ok, _} -> {:cont, acc}
            {:error, reason} -> {:halt, {:error, reason}}
          end
        end)

      case add_impl_result do
        nil -> {:ok, ""}
        {:error, reason} -> {:error, reason}
      end
    else
      {:error, reason} -> {:error, reason}
    end
  end

  def add_runner_with_impls(_invalid) do
    {:error, "Invalid request"}
  end

  def remove_runner(%{
        "runner" => %{"name" => r_name, "address" => r_addr} = runner
      })
      when is_binary(r_name) and is_binary(r_addr) do
    import Ecto.Query

    with {:ok, runner_id} <- runner_id(runner) do
      remove_impls(runner_id)

      case Repo.delete_all(from r in Runner, where: r.id == ^runner_id) do
        {1, _} -> {:ok, ""}
        _ -> {:error, :db_error}
      end
    else
      {:error, reason} -> {:error, reason}
    end
  end

  def remove_runner(_invalid) do
    {:error, "Invalid request"}
  end

  defp runner_id(runner) do
    import Ecto.Query

    case Repo.one(
           from r in Runner,
             where: r.name == ^runner["name"] and r.address == ^runner["address"],
             select: r.id
         ) do
      nil -> {:error, "Runner not found"}
      id -> {:ok, id}
    end
  end

  defp add_runner(runner) do
    insert_result =
      %Runner{}
      |> Runner.changeset(runner)
      |> Repo.insert()

    case insert_result do
      {:ok, changeset} ->
        {:ok, changeset.id}

      {:error, %Ecto.Changeset{errors: [name: {"has already been taken", _}]}} ->
        {:error, "Runner " <> runner["name"] <> " already exists"}

      {:error, %Ecto.Changeset{errors: [address: {"has already been taken", _}]}} ->
        {:error, "Runner with address " <> runner["address"] <> " already exists"}

      {:error, changeset} ->
        Logger.error("Error inserting runner to db: #{inspect(changeset.errors)}")
        {:error, :db_error}
    end
  end

  defp add_impl(impl_name, runner_id) do
    impl =
      %Impl{}
      |> Impl.changeset(%{"name" => impl_name, "runner_id" => runner_id})
      |> Repo.insert()

    case impl do
      {:ok, _} -> {:ok, ""}
      {:error, _} -> {:error, :db_error}
    end
  end

  defp validate_impls(impls) do
    all_strings = Enum.all?(impls, &is_binary/1)
    all_unqiue = Enum.uniq(impls) == impls

    case {all_strings, all_unqiue} do
      {true, true} -> {:ok, impls}
      {false, _} -> {:error, "Invalid request"}
      {_, false} -> {:error, "Impls must be distinct"}
    end
  end

  defp remove_impls(runner_id) do
    import Ecto.Query

    Repo.delete_all(from i in Impl, where: i.runner_id == ^runner_id)
  end
end
