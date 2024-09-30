<script lang="ts">
    import { Chart } from "chart.js/auto";
    import { onMount } from "svelte";
    import { chart_data } from "../charDataStore";
    import { fib_impls, type ApiResponse } from "../data";

    let canvas: HTMLCanvasElement;
    let chart: Chart;

    $: if (chart != undefined) {
        const data = $chart_data;
        data.forEach((data, impl) => {
            for (let i = 0; i < data.length; i++) {
                // @ts-ignore
                chart.options.scales.x.max = Math.max(
                    // @ts-ignore
                    chart.options.scales.x.max as number,
                    data[i].x,
                );
            }
            let idx = fib_impls.findIndex((i) => i == impl);
            chart.data.datasets[idx].data = data;
        });
        chart.update();
    }

    function loadCalculated() {
        for (let i of fib_impls) {
            fetch(`/api/getall/${i}`).then(async (resp) => {
                let json: ApiResponse[] = await resp.json();

                for (let r of json) {
                    chart_data.addPoint(i, { x: r.idx, y: r.ms });
                }
                chart.update();
            });
        }
    }

    function deleteAll() {
        for (let i of fib_impls) {
            fetch(`/api/deleteall/${i}`);
            chart_data.clear(i);
        }
    }

    onMount(() => {
        let createDatasets = () => {
            let arr = [];
            for (let i of fib_impls) {
                arr.push({
                    label: i,
                    data: [],
                });
            }
            return arr;
        };

        chart = new Chart(canvas, {
            type: "scatter",
            data: {
                datasets: createDatasets(),
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

        loadCalculated();
    });
</script>

<div class="bg-zinc-800 shadow-md rounded-lg p-6">
    <h2 class="text-xl font-semibold text-zinc-100 mb-4">
        Execution Time Graph
    </h2>
    <canvas bind:this={canvas} class="w-full h-64"></canvas>

    <div class="flex justify-end">
        <button
            on:click={deleteAll}
            class="bg-teal-500 text-white font-semibold py-2 px-4 rounded-md shadow hover:bg-teal-600 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-teal-400 focus:ring-offset-zinc-800 mt-6"
        >
            Delete data
        </button>
    </div>
</div>
