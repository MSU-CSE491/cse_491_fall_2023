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

		/**
		 * Adds an item to the agents inventory if there's room
		 * @param item shared_ptr to item being added
		 * @return bool if the operation was successful
		 */
		bool AddItem(std::shared_ptr<Item> item){
			if(mItemCount < 10){
				for(int i = 0; i < mItems.size(); i++){
					if(mItems[i] == nullptr){
						mItems[i] = item;
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
			for(int i = 0; i < mItems.size(); i++){
				if(mItems[i] == item){
					mItems[i] = nullptr;
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
		bool HasItem(std::shared_ptr<Item> item){
			for(int i = 0; i < mItems.size(); i++){
				if(mItems[i] == item){
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
			for(auto item : mItems){
				if(item.get() != nullptr){
					std::cout << "Stored Item: " << item.get() << "\n";
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
