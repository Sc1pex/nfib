defmodule NfibServerWeb.IconComponents do
  use Phoenix.Component

  attr :class, :string, default: nil

  def online_icon(assigns) do
    ~H"""
    <svg
      width="50"
      height="50"
      viewBox="0 0 50 50"
      fill="none"
      xmlns="http://www.w3.org/2000/svg"
      class={@class}
    >
      <circle cx="25" cy="25" r="8" fill="currentColor" />
      <circle cx="25" cy="25" r="8" stroke="currentColor" stroke-width="2" fill="none">
        <animate attributeName="r" from="8" to="20" dur="1.5s" repeatCount="indefinite" />
        <animate attributeName="opacity" from="1" to="0" dur="1.5s" repeatCount="indefinite" />
      </circle>
    </svg>
    """
  end
end
