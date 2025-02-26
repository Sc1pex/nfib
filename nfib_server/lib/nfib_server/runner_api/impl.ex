defmodule NfibServer.RunnerAPI.Impl do
  use Ecto.Schema
  import Ecto.Changeset

  schema "impls" do
    field :name, :string
    field :runner_id, :id

    timestamps(type: :utc_datetime)
  end

  @doc false
  def changeset(impl, attrs) do
    impl
    |> cast(attrs, [:name, :runner_id])
    |> validate_required([:name, :runner_id])
  end
end
