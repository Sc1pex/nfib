<script context="module" lang="ts">
    export type DataPoint = {
        x: number;
        y: number;
    };
</script>

<script lang="ts">
    import { Chart } from "chart.js/auto";
    import { onMount } from "svelte";

    export let data: DataPoint[];

    let canvas: HTMLCanvasElement;
    let chart: Chart;

    $: if (chart != undefined) {
        chart.data.datasets[0].data = data;
        for (let i = 0; i < data.length; i++) {
            chart.options.scales.x.max = Math.max(
                chart.options.scales.x.max as number,
                data[i].x,
            );
        }
        chart.update();
    }

    onMount(() => {
        chart = new Chart(canvas, {
            type: "scatter",
            data: {
                datasets: [
                    {
                        data: [],
                    },
                ],
            },
            options: {
                scales: {
                    x: {
                        type: "linear",
                        max: 1,
                    },
                    y: {
                        beginAtZero: true,
                        title: {
                            display: true,
                            text: "Time (ms)",
                        },
                    },
                },
                plugins: {
                    legend: {
                        display: false,
                    },
                },
                elements: {
                    point: {
                        radius: 1,
                    },
                },
                animation: {
                    duration: 250,
                },
            },
        });
    });
</script>

<div class="bg-zinc-800 shadow-md rounded-lg p-6">
    <h2 class="text-xl font-semibold text-zinc-100 mb-4">
        Execution Time Graph
    </h2>
    <canvas bind:this={canvas} class="w-full h-64"></canvas>
</div>
