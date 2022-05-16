load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")

def camera_deps():
    if "glm" not in native.existing_rules():
        new_git_repository(
            name = "glm",
            build_file = "//third_party:glm.BUILD",
            commit = "cc98465e3508535ba8c7f6208df934c156a018dc",
            remote = "https://github.com/g-truc/glm.git",
        )
