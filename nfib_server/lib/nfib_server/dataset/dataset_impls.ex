defmodule NfibServer.Dataset.DatasetImpls do
  use Ecto.Schema
  import Ecto.Changeset

  schema "dataset_impls" do

    field :dataset_id, :id
    field :impl_id, :id

    timestamps(type: :utc_datetime)
  end

  @doc false
  def changeset(dataset_impls, attrs) do
    dataset_impls
    |> cast(attrs, [])
    |> validate_required([])
  end
end
