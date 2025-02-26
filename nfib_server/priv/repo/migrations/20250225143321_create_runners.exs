defmodule NfibServer.Repo.Migrations.CreateRunners do
  use Ecto.Migration

  def change do
    create table(:runners) do
      add :name, :string
      add :address, :string

      timestamps(type: :utc_datetime)
    end

    create unique_index(:runners, [:address])
    create unique_index(:runners, [:name])
  end
end
