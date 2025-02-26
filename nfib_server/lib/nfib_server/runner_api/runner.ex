defmodule NfibServer.RunnerAPI.Runner do
  use Ecto.Schema
  import Ecto.Changeset

  schema "runners" do
    field :name, :string
    field :address, :string

    timestamps(type: :utc_datetime)
  end

  @doc false
  def changeset(runner, attrs) do
    runner
    |> cast(attrs, [:name, :address])
    |> validate_required([:name, :address])
    |> unique_constraint(:address)
    |> unique_constraint(:name)
  end
end
