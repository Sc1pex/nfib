import { writable } from "svelte/store";
import type { DataPoint } from "./data";

function createChartData() {
  const { subscribe, set, update } = writable<DataPoint[]>([]);

  return {
    subscribe,
    addPoint: (point: DataPoint) => {
      update((p) => {
        p.push(point);
        return p;
      });
    },
    clear: () => {
      set([]);
    },
  };
}

export const chart_data = createChartData();
