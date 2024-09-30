<script context="module" lang="ts">
    export type FibEvent = {
        n: number;
        result: string;
        time: number;
        impl: string;
    };
</script>

<script lang="ts">
    import { createEventDispatcher } from "svelte";
    import { fib_impls } from "../data";

    let impl: string;
    let start: number = 1;
    let end: number = 100;

    const dispatch = createEventDispatcher<{ fib: FibEvent }>();

    function handleSubmit() {
        const get_num = async (num: number) => {
            try {
                let resp = await fetch(`/api/calculate/${impl}/${num}`);
                let json = await resp.json();

                dispatch("fib", {
                    n: num,
                    result: json.num,
                    time: json.ms,
                    impl,
                });

                if (num < end) {
                    setTimeout(get_num, 10, num + 1);
                }
            } catch (e) {
                console.log("error: ", e);
            }
        };
        get_num(start);
    }
</script>

<div class="bg-zinc-800 shadow-md rounded-lg p-6 mb-8">
    <form on:submit|preventDefault={handleSubmit}>
        <h1 class="text-2xl font-semibold text-zinc-100 mb-4">
            Fibonacci Time Calculator
        </h1>

        <div class="grid grid-cols-1 md:grid-cols-3 gap-4 mb-4">
            <div>
                <label
                    for="startNumber"
                    class="block text-sm font-medium text-zinc-300"
                    >Start Number</label
                >
                <input
                    type="number"
                    id="startNumber"
                    name="startNumber"
                    class="mt-1 p-2 block w-full border border-zinc-600 rounded-md shadow-sm focus:ring-teal-400 focus:border-teal-400 bg-zinc-700 text-zinc-100 sm:text-sm"
                    bind:value={start}
                />
            </div>

            <div>
                <label
                    for="endNumber"
                    class="block text-sm font-medium text-zinc-300"
                    >End Number</label
                >
                <input
                    type="number"
                    id="endNumber"
                    name="endNumber"
                    class="mt-1 p-2 block w-full border border-zinc-600 rounded-md shadow-sm focus:ring-teal-400 focus:border-teal-400 bg-zinc-700 text-zinc-100 sm:text-sm"
                    bind:value={end}
                />
            </div>

            <div>
                <label
                    for="implementation"
                    class="block text-sm font-medium text-zinc-300"
                    >Implementation</label
                >
                <select
                    id="implementation"
                    name="implementation"
                    class="mt-1 block w-full p-2 border border-zinc-600 bg-zinc-700 rounded-md shadow-sm focus:ring-teal-400 focus:border-teal-400 text-zinc-100 sm:text-sm"
                    bind:value={impl}
                >
                    {#each fib_impls as option}
                        <option value={option}>
                            {option}
                        </option>
                    {/each}
                </select>
            </div>
        </div>

        <div class="flex justify-end">
            <button
                type="submit"
                class="bg-teal-500 text-white font-semibold py-2 px-4 rounded-md shadow hover:bg-teal-600 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-teal-400 focus:ring-offset-zinc-800"
            >
                Calculate
            </button>
        </div>
    </form>
</div>
