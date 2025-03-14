defmodule NfibServer.Dataset do
  use Ecto.Schema
  import Ecto.Changeset

  schema "datasets" do
    field :name, :string

    timestamps(type: :utc_datetime)
  end

  @doc false
  def changeset(dataset, attrs) do
    dataset
    |> cast(attrs, [:name])
    |> validate_required([:name])
    |> unique_constraint(:name)
  end
end
