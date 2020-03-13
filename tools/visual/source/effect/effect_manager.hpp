#ifndef ITL_EFFECT_MGR_HPP
#define ITL_EFFECT_MGR_HPP

#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <vector>

#include "../config/config.hpp"
#include "../logger/logger.hpp"
#include "../math/math.hpp"
#include "../perlin_noise/perlin_noise.hpp"
#include "../transform/transform.hpp"

namespace itl
{
    /**
     * class which manages effects
     */
    class EffectManager
    {
       public:
        /**
         * @brief explicit ctor
         * @param log - pointer to logger
         * @param path_to_data - path to data folder
         */
        explicit EffectManager(const std::shared_ptr<Logger>& log,
                               const std::string& path_to_data);

        /**
         * @brief generates all possible sprites in use of defined effects
         * @param sprite - matrix which represents sprite
         * @return vector of produced sprites
         */
        std::vector<std::shared_ptr<cv::Mat>> generateSprites(
            const cv::Mat& sprite);

       private:
        /**
         * enum which describes types of effects
         */
        enum class function_t
        {
            rotate,  ///< rotating effect (center)
            scale,   ///< scaling effect (linear)
            perlin   ///< noising efffect (perlin)
        };

        /**
         * @brief init available effects
         */
        void load_functions();

        /**
         * @brief fills indexes of effects' packs
         * @see functions
         */
        void generate_all_effect_packs();

        std::map<function_t, std::vector<std::function<void(cv::Mat&)>>>
            functions;  ///< map which contains all available functions
        ///< with given keys stored as vector

        std::vector<std::vector<int>>
            pack_indexes;  ///< packIndexes[i][j] is index of j'th effect from
                           ///< i'th pack

        std::unique_ptr<Transform> transform;  ///< ptr to transform object

        std::unique_ptr<PerlinNoise>
            perlin_noise;  ///< ptr to perlin noise generator

        std::shared_ptr<Logger> logger;  ///< ptr to logger
    };
}  // namespace itl

#endif