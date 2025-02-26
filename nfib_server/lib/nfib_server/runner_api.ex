defmodule NfibServer.RunnerAPI do
  require Logger

  alias NfibServer.Repo
  alias NfibServer.RunnerAPI.{Runner, Impl}

  def add_runner_with_impls(%{
        "runner" => %{"name" => _, "address" => _} = runner,
        "impls" => impls
      })
      when is_list(impls) do
    with {:ok, _} <- validate_impls(impls),
         {:ok, runner_id} <- add_runner(runner) do
      IO.puts("Runner ID:")
      IO.inspect(runner_id)

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
    runner =
      %Runner{}
      |> Runner.changeset(runner)
      |> Repo.insert()

    case runner do
      {:ok, runner} ->
        {:ok, runner.id}

      {:error, %Ecto.Changeset{errors: errors}} ->
        if {:name, ["has already been taken"]} in errors do
          {:ok, Repo.get_by(Runner, name: runner.name).id}
        else
          Logger.info("Runner errors: #{inspect(errors)}")
          {:error, "Invalid request"}
        end
    end
  end

  defp add_impl(impl_name, runner_id) do
    impl =
      %Impl{}
      |> Impl.changeset(%{"name" => impl_name, "runner_id" => runner_id})
      |> Repo.insert()

    case impl do
      {:ok, _} -> {:ok, ""}
      {:error, err} -> {:error, "Invalid impl" <> impl_name}
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
