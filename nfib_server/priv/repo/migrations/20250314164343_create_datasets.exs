defmodule NfibServer.Repo.Migrations.CreateDatasets do
  use Ecto.Migration

  def change do
    create table(:datasets) do
      add :name, :string

      timestamps(type: :utc_datetime)
    end

    create unique_index(:datasets, [:name])
  end
end
