#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

class PotionRecipe
{
public:
	PotionRecipe(const std::string& potionName, const std::string& coreIngredient)
	: potionName_(potionName), coreIngredient_(coreIngredient) {}

	const std::string& GetPotionName() const { return potionName_; }
	const std::string& GetCoreIngredient() const { return coreIngredient_; }

private:
	std::string potionName_;
	std::string coreIngredient_;
};

class AlchemyWorkshop
{
public:
	void AddRecipe(const std::string& potionName, const std::string& coreIngredient)
	{
		recipes_.emplace_back(potionName, coreIngredient);
		inventory_[potionName] = 3;
	}

	void DisplayAllRecipes() const
	{
		for (const auto& recipe : recipes_)
		{
			std::cout << "Potion: " << recipe.GetPotionName()
			   	<< "| Ingredient: " << recipe.GetCoreIngredient()
				<< "| Stock: " << inventory_.at(recipe.GetPotionName())
				<< "\n";
		}
	}

	PotionRecipe SearchRecipeByName(const std::string& name) const
	{
		for (const auto& recipe : recipes_)
		{
			if (recipe.GetPotionName() == name)
				return recipe;
		}
		throw std::runtime_error("해당 이름의 레시피가 없습니다.");
	}

	std::vector<PotionRecipe> SearchRecipeByIngredient(const std::string& ingredient) const
	{
		std::vector<PotionRecipe> result;
		for (const auto& recipe : recipes_)
		{
			if (recipe.GetCoreIngredient() == ingredient)
				result.push_back(recipe);
		}
		return result;
	}

	std::vector<PotionRecipe> SearchByNameOrIngredient(const std::string& keyword) const
	{
		std::vector<PotionRecipe> result;
		for (const auto& recipe : recipes_)
		{
			if (recipe.GetPotionName() == keyword || recipe.GetCoreIngredient() == keyword)
				result.push_back(recipe);
		}
		return result;
	}

	bool GivePotion(const std::string& potionName)
	{
		if (inventory_[potionName] > 0) {
			inventory_[potionName]--;
			std::cout << potionName << " 지급 완료! 남은 재고: " 
				      << inventory_[potionName] << "\n";
			return true;
		} else {
			std::cout << potionName << " 재고가 부족합니다.\n";
			return false;
		}
	}

	void ReturnPotion(const std::string& potionName)
	{
		if (inventory_[potionName] < 3) {
			inventory_[potionName]++;
			std::cout << potionName << " 반환 완료! 현재 재고: " << inventory_[potionName] << "\n";
		} else {
		   std::cout << potionName << " 재고가 이미 최대치(3)입니다.\n" << "\n";
		}
	}

private:
	std::vector<PotionRecipe> recipes_;
	std::unordered_map<std::string, int> inventory_;
};

int main()
{
	AlchemyWorkshop workshop;

	workshop.AddRecipe("Healing Potion", "Herb");
	workshop.AddRecipe("Mana Potion", "Magic Water");
	workshop.AddRecipe("Stamina Potion", "Herb");
	workshop.AddRecipe("Fire Resistance Potion", "Fire Flower");

	std::cout << "=== 초기 상태 ===\n";
	workshop.DisplayAllRecipes();

	try {
		PotionRecipe recipe = workshop.SearchRecipeByName("Mana Potion");
		std::cout << "검색된 레시피: " << recipe.GetPotionName() << std::endl;
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}

	std::vector<PotionRecipe> herbRecipes = workshop.SearchRecipeByIngredient("Herb");
	std::cout << "Herb를 사용하는 레시피 수: " << herbRecipes.size() << std::endl;

	std::cout << "\n=== 지급 테스트 ===\n";
	workshop.GivePotion("Healing Potion");
	workshop.GivePotion("Healing Potion");
	workshop.GivePotion("Healing Potion");
	workshop.GivePotion("Healing Potion");

	std::cout << "\n=== 반환 테스트 ===\n";
	workshop.ReturnPotion("Healing Potion");
	workshop.ReturnPotion("Healing Potion");
	workshop.ReturnPotion("Healing Potion");
	workshop.ReturnPotion("Healing Potion");

	std::cout << "\n=== 최종 상태 ===\n";
	workshop.DisplayAllRecipes();
	// 홀리몰리
	return 0;
}