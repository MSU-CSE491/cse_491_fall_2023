import sys
import pandas as pd
import json
import matplotlib.pyplot as plt
import numpy as np 

class ItemGraphPlotter:
    def __init__(self, json_file):
        with open(json_file, 'r') as file:
            json_data = json.load(file, parse_float=self._parse_float)
        self.json_data = json_data

    @staticmethod
    def _parse_float(obj):
        try:
            if obj == 'Infinity':
                return float('inf')
            return float(obj)
        except (ValueError, TypeError):
            return obj

    def plot_item_usage_bar_graph(self):
        items = self.json_data.get("items", [])
        if not items:
            print("No 'items' key found in the JSON data.")
            return

        names = [item.get("name", "") for item in items]
        uses = [item.get("amountOfUses", 0) for item in items]

        plt.bar(names, uses, color='skyblue')
        plt.title("Item Usage Bar Graph")
        plt.xlabel("Items")
        plt.ylabel("Amount of Uses")
        plt.show()

    def plot_item_damage_line_graph(self):
        items = self.json_data.get("items", [])
        if not items:
            print("No 'items' key found in the JSON data.")
            return

        for item in items:
            name = item.get("name", "")
            damages = item.get("damages", [])

            plt.plot(damages, label=name)

        plt.title("Item Damage Line Graph")
        plt.xlabel("Damage Instances")
        plt.ylabel("Damage")
        plt.legend()
        plt.show()

    def plot_agent_path(self, agent_name):
        agents = self.json_data.get("agents", [])
        if not agents:
            print("No 'agents' key found in the JSON data.")
            return

        for agent in agents:
            if agent.get("agentName") == agent_name:
                agent_positions = agent.get("positions", [])
                if not agent_positions:
                    print(f"No 'positions' data found for agent '{agent_name}'.")
                    return

                df = pd.DataFrame(agent_positions)

                heatmap, xedges, yedges = np.histogram2d(df["x"], df["y"], bins=(75, 75))
                extent = [xedges[0], xedges[-1], yedges[0], yedges[-1]]
                plt.imshow(heatmap.T, extent=extent, origin='lower', cmap='viridis')
                plt.title(f"Heatmap - Agent: {agent_name}")
                plt.colorbar(label='Frequency')
                plt.xlabel("X")
                plt.ylabel("Y")
                plt.grid(True)
                plt.show()

                return

        print(f"Agent with name '{agent_name}' not found in the JSON data.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python ItemGraphPlotter.py <json_file>")
        sys.exit(1)

    json_file = sys.argv[1]
    try:
        item_graph_plotter = ItemGraphPlotter(json_file)

        agent_name = "Interface"
        item_graph_plotter.plot_agent_path(agent_name)

        # Determine which graph to plot based on the file content
        if "amountOfUses" in item_graph_plotter.json_data.get("items", [])[0]:
            item_graph_plotter.plot_item_usage_bar_graph()
        elif "damages" in item_graph_plotter.json_data.get("items", [])[0]:
            item_graph_plotter.plot_item_damage_line_graph()
        else:
            print("Invalid JSON file format for graphing.")

    except FileNotFoundError:
        print(f"Error: File '{json_file}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")