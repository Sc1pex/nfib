defmodule NfibServer.Repo.Migrations.CreateDatasetImpls do
  use Ecto.Migration

  def change do
    create table(:dataset_impls) do
      add :dataset_id, references(:datasets, on_delete: :nothing)
      add :impl_id, references(:impls, on_delete: :nothing)

      timestamps(type: :utc_datetime)
    end

    create index(:dataset_impls, [:dataset_id])
    create index(:dataset_impls, [:impl_id])
  end
end
