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

		/// Gets the current item count
		int GetItemCount() const { return mItemCount; }

		/**
		 * Adds an item to the agents inventory if there's room
		 * @param item shared_ptr to item being added
		 * @return bool if the operation was successful
		 */
		bool AddItem(std::shared_ptr<Item> item){
			if(mItemCount < 10){
				for(auto& stored : mItems){
					if(stored.get() == nullptr){
						stored = item;
						mItemCount++;
						return true;
					}
				}
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
			for(auto& stored : mItems){
				if(stored.get() == item.get()){
					stored.reset();
					mItemCount--;
					return true;
				}
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
				if(stored.get() == item.get()){
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




    };
}
