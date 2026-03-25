#include <string>
#include <vector>
#include <map>
#include <iostream>

// PotionRecipe class 정의
class PotionRecipe
{
public:
    // 멤버 변수들을 초기화하는 생성자
    PotionRecipe(const std::string& name, const std::vector<std::string>& ingredients)
        : name_(name), ingredients_(ingredients) {}

    // 물약 이름을 반환
    const std::string& GetName() const;
    // 물약을 만들기 위한 재료 목록을 반환
    const std::vector<std::string>& GetIngredients() const;

private:
    std::string name_;
    std::vector<std::string> ingredients_;
};

// RecipeManager class 정의
class RecipeManager
{
public:
    // 레시피 추가 후, 내부에 저장된 레시피의 포인터를 반환(없으면 nullptr)
    // - 예외처리 : 이미 있는 Recipe을 추가할 때, 자유롭게 처리
    PotionRecipe* AddRecipe(const std::string& name, const std::vector<std::string>& ingredients);

    // 이름으로 1개 찾기(없으면 nullptr)
    PotionRecipe* FindRecipeByName(const std::string& name);

    // 재료로 여러 개 찾기(결과 복사본 반환)
    std::vector<PotionRecipe> FindRecipesByIngredient(const std::string& ingredient) const;

    // 전체 레시피(읽기 전용 참조)
    const std::vector<PotionRecipe>& GetAllRecipes() const;

private:
    std::vector<PotionRecipe> recipes_;
};

// StockManager class 정의
class StockManager
{
public:
    static constexpr int MAX_STOCK = 3;

    // 레시피 추가 시 자동으로 3개 재고 세팅
    void InitializeStock(const std::string& potionName);

    // 재고가 1개 이상이면 1개 지급(감소)하고 true, 아니면 false
    bool DispensePotion(const std::string& potionName);

    // 공병 반환(재고 +1).
    // - 예외처리 : 단, MAX_STOCK 초과 불가
    void ReturnPotion(const std::string& potionName);

    // 현재 재고 조회(없으면 0)
    int GetStock(const std::string& potionName) const;

private:
    std::map<std::string, int> potionStock_;
};

// AlchemyWorkshop class 정의
class AlchemyWorkshop
{
public:
    // 레시피 추가 + 자동 재고 3개 생성
    // (중복 이름 처리 방식은 구현 자유: 덮어쓰기/거부/중복허용 등)
    void AddRecipe(const std::string& potionName, const std::vector<std::string>& ingredients);

    // 레시피/재고 출력(형식은 자유)
    void DisplayAllRecipes() const;

    // 재고 조회 (이름)
    int GetStockByName(const std::string& potionName) const;

    // potionName으로 검색하여 재고 있으면 지급처리하고 true, 재고 없으면 false
    bool DispensePotionByName(const std::string& potionName);

    // 해당 재료를 포함하는 레시피들 중, 재고 있는 것들을 지급처리
    // 반환: 실제로 지급된 물약 이름 목록(없으면 빈 벡터)
    std::vector<std::string> DispensePotionsByIngredient(const std::string& ingredient);

    // potionName에 공병을 반환 처리(최대 3개 제한)
    void ReturnPotionByName(const std::string& potionName);

private:
    RecipeManager recipeManager_;
    StockManager stockManager_;
};

void main()
{
    AlchemyWorkshop workshop;

    workshop.AddRecipe("Healing Potion", { "Herb", "Water" });
    workshop.AddRecipe("Mana Potion", { "Magic Water", "Crystal" });
    workshop.AddRecipe("Stamina Potion", { "Herb", "Berry" });
    workshop.AddRecipe("Fire Resistance Potion", { "Fire Flower", "Ash" });

    std::cout << "=== 초기 상태 (레시피 추가 + 재고 자동 3개) ===\n";
    workshop.DisplayAllRecipes();

    std::cout << "\n[재고 확인] Healing Potion 재고: "
        << workshop.GetStockByName("Healing Potion") << "\n";

    std::cout << "\n=== 이름으로 지급 테스트 (Healing Potion 3회 지급) ===\n";
    std::cout << "1회 지급: " << (workshop.DispensePotionByName("Healing Potion") ? "성공" : "실패") << "\n";
    std::cout << "2회 지급: " << (workshop.DispensePotionByName("Healing Potion") ? "성공" : "실패") << "\n";
    std::cout << "3회 지급: " << (workshop.DispensePotionByName("Healing Potion") ? "성공" : "실패") << "\n";

    std::cout << "현재 재고: " << workshop.GetStockByName("Healing Potion") << "\n";

    std::cout << "4회 지급(재고 없으면 실패): "
        << (workshop.DispensePotionByName("Healing Potion") ? "성공" : "실패") << "\n";

    std::cout << "\n=== 재료로 지급 테스트 (ingredient = Herb) ===\n";
    std::vector<std::string> dispensed = workshop.DispensePotionsByIngredient("Herb");

    std::cout << "지급된 물약 수: " << dispensed.size() << "\n";
    for (const std::string& name : dispensed)
        std::cout << "- " << name << "\n";

    std::cout << "\n=== 공병 반환 테스트 (Healing Potion) ===\n";
    workshop.ReturnPotionByName("Healing Potion");
    workshop.ReturnPotionByName("Healing Potion");
    workshop.ReturnPotionByName("Healing Potion"); // 이미 3이면 더 올라가면 안 됨

    std::cout << "반환 후 재고(최대 3 유지): "
        << workshop.GetStockByName("Healing Potion") << "\n";

    std::cout << "\n=== 최종 상태 ===\n";
    workshop.DisplayAllRecipes();
}

