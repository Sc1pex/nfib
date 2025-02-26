defmodule NfibServer.Repo.Migrations.CreateImpls do
  use Ecto.Migration

  def change do
    create table(:impls) do
      add :name, :string
      add :runner_id, references(:runners, on_delete: :nothing)

      timestamps(type: :utc_datetime)
    end

    create index(:impls, [:runner_id])
  end
end
