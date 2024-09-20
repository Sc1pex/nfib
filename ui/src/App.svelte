<script lang="ts">
    const options = ["CNaive", "CMatrix"];
    let selected: string;
    let num: number;

    let result: { num: string; ms: number } | undefined = undefined;

    function handleSubmit() {
        fetch(`/api/calculate/${selected}/${num}`).then(async (resp) => {
            const res = await resp.json();
            result = res;
        });
    }
</script>

<form
    class="flex flex-col items-center pt-20 gap-5"
    on:submit|preventDefault={handleSubmit}
>
    <select class="bg-zinc-600 p-2 rounded" bind:value={selected}>
        {#each options as option}
            <option value={option}>
                {option}
            </option>
        {/each}
    </select>

    <input
        class="bg-zinc-600 w-16 p-2 rounded"
        type="number"
        bind:value={num}
        min="0"
    />

    <button type="submit"> Calculate </button>
</form>
