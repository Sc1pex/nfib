import { writable } from "svelte/store";
import type { DataPoint } from "./data";

function createChartData() {
  const { subscribe, update } = writable<Map<string, DataPoint[]>>(new Map());

  return {
    subscribe,
    addPoint: (impl: string, point: DataPoint) => {
      update((p) => {
        let d = p.get(impl);
        if (d === undefined) {
          p.set(impl, [point]);
        } else {
          d.push(point);
        }
        return p;
      });
    },
    clear: (impl: string) => {
      update((p) => {
        p.delete(impl);
        return p;
      });
    },
  };
}

export const chart_data = createChartData();
