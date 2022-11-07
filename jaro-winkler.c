#include "batch_jaro_winkler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char      *candidates[] = { "hello", "hiz" };
    uint32_t  candidates_lengths[] = { 5, 3 };
    uint32_t  exportable_model_size;
    uint32_t  nb_results;

    // char_width = 1 ; nb_candidates = 2 ; nb_runtime_threads = 1
    void *exportable_model = bjw_build_exportable_model(
      candidates, 1, candidates_lengths, 2, NULL, 1, &exportable_model_size
    );
    if (!exportable_model)
        exit(1);
    void *runtime_model = bjw_build_runtime_model(exportable_model);
    if (!runtime_model)
        exit(1);

    // input_length = 5 ; min_score = -1.0 (deactivate)
    // weight = 0.1 (default value for standard jaro winkler)
    // threshold = 0.7 (default value for standard jaro winkler)
    // n_best_results = 0 (deactivate)
    bjw_result *res = bjw_jaro_winkler_distance(runtime_model, "hallo", 5, -1.0, 0.1, 0.7, 0, &nb_results);
    if (!res)
        exit(1);

    uint32_t best_candidate_ind = 0;
    for (uint32_t i_res = 0; i_res < nb_results; i_res++)
    {
        // Warning: candidates are not null terminated, as the meaning of bytes within candidates
        // depends on the encoding, including for 0.
        printf(
            "{ .candidate = \"%.*s\", .score = %f }\n",
            res[i_res].candidate_length, res[i_res].candidate, res[i_res].score
        );
        if (res[i_res].score > res[best_candidate_ind].score)
            best_candidate_ind = i_res;
    }

    // Important: the 'candidate' field in `bjw_result` is a pointer to somewhere within the exportable model.
    // If you want to keep candidates after the exportable model is being freed, you must copy the data.
    // char_width = 1
    char *best_candidate = malloc(res[best_candidate_ind].candidate_length * 1);
    memcpy(best_candidate, res[best_candidate_ind].candidate, res[best_candidate_ind].candidate_length * 1);
    uint32_t best_candidate_length = res[best_candidate_ind].candidate_length;

    free(res);
    bjw_free_runtime_model(runtime_model);
    free(exportable_model);

    printf("best candidate: \"%.*s\"\n", best_candidate_length, best_candidate);
    free(best_candidate);
    return (0);
}
