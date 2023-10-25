/**
 * This file is part of the Fall 2023, CSE 491 Course Project - Group 1 Fork - yousif_m Branch.
 * @brief An Inventory class that holds items for the Agent
 * @note Status: PROPOSAL
 * @author Yousif Murrani
 **/

#pragma once

#include <vector>
#include <memory>
#include <iostream>

#include "Item.hpp"

namespace cse491{
	class AgentBase;
}

namespace walle {

    class Inventory {

    private:

        /// Agent this inventory is a part of
        cse491::AgentBase* mAgent = nullptr;

		/// max items that this inventory can hold
		int mMaxItems = 10;

        /// amount of items the agent currently has
        int mItemCount = 0;

		/// vector of items that the inventory holds
		std::vector<std::shared_ptr<Item>> mItems;


    public:

        Inventory() {}				///< constructor
        ~Inventory() = default; 	///< destructon

		/// function to set the agent for this inventory
		void SetAgent(cse491::AgentBase* agent) { mAgent = agent; }

		/// function to get the agent for this inventory
		cse491::AgentBase* GetAgent() const { return mAgent; }

		/// set the maximum items for this inventory
		void SetMaxItems(const int max) { mMaxItems = max; }

		/// Gets the current item count
		int GetItemCount() const { return mItemCount; }

		/**
		 * Adds an item to the agents inventory if there's room
		 * @param item shared_ptr to item being added
		 * @return bool if the operation was successful
		 */
		bool AddItem(std::shared_ptr<Item> item){
			if(mItemCount < mMaxItems){
				mItems.push_back(item);
				mItemCount++;
				return true;
			}
			else{
				return false;
			}
		}

		/**
		 * Drops a specific item from the inventory
		 * @param item to be dropped
		 * @return bool if operation worked
		 */
		bool DropItem(std::shared_ptr<Item> item){
			// find items location in mItems
			auto it = std::remove(mItems.begin(), mItems.end(), item);

			// if its an item, delete it
			if(it != mItems.end()){
				mItems.erase(it);
				mItemCount--;
				return true;
			}
			return false;
		}

		/**
		 * Checks if the agent has an item
		 * @param item
		 * @return
		 */
		bool HasItem(const std::shared_ptr<Item> item){
			for(const auto& stored : mItems){
				if(stored == item){
					return true;
				}
			}
			return false;
		}

		/// clears the inventory and resets item count
		void ClearInventory(){
			mItems.clear();
			mItemCount = 0;
		}

		/// displays the current inventory
		void DisplayInventory(){
			for(auto& item : mItems){
				if(item.get() != nullptr){
					std::cout << "Stored Item: " << item.get()->GetName() << "\n";
				}
			}
		}

		/// Transfer a specific item to a new inventory
		bool TransferItem(std::shared_ptr<Item> item, Inventory& newInventory){
			if(HasItem(item)){
				if(newInventory.AddItem(item)){
					DropItem(item);
					return true;
				}
				return false;
			}
			return false;
		}

        /// Calculate the total value of the items in the inventory
        int CalculateInventoryValue(){
            int total_value = 0;
            for(auto item : mItems){
                total_value += item->GetValue();
            }
            return total_value;
        }

        /// Calculate the total weight of the items in the inventory
        double CalculateInventoryWeight(){
            double total_weight = 0.0;
            for(auto item : mItems){
                total_weight += item->GetWeight();
            }
            return total_weight;
        }


    };
}
