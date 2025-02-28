defmodule NfibServer.RunnerAPI do
  require Logger

  alias NfibServer.Repo
  alias NfibServer.RunnerAPI.{Runner, Impl}

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
end
