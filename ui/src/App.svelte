<script lang="ts">
    import Input, { type FibEvent } from "./components/Input.svelte";
    import Chart from "./components/Chart.svelte";
    import { chart_data } from "./charDataStore";

    function handleResult(e: CustomEvent<FibEvent>) {
        chart_data.addPoint({
            x: e.detail.n,
            y: e.detail.time,
        });
    }
</script>

<div class="max-w-4xl mx-auto p-6">
    <Input on:fib={handleResult} />

    <Chart />
</div>
