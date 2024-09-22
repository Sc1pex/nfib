<script lang="ts">
    import Input, { type FibEvent } from "./components/Input.svelte";
    import Chart, { type DataPoint } from "./components/Chart.svelte";

    let chart_data: DataPoint[] = [];

    function handleResult(e: CustomEvent<FibEvent>) {
        chart_data.push({
            x: e.detail.n,
            y: e.detail.time,
        });
        chart_data = chart_data;
    }
</script>

<div class="max-w-4xl mx-auto p-6">
    <Input on:fib={handleResult} />

    <Chart data={chart_data} />
</div>
