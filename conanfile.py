import os

from conans import ConanFile, tools


class MemoizedmemberConan(ConanFile):
    name = "memoized_member"
    version = "1.2"
    license = "MIT"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Memoizedmember here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    settings = "os", "compiler", "arch", "build_type"
    exports_sources = "include/*"
    no_copy_source = True
    # No settings/options are necessary, this is header only

    build_requires = ["gtest/1.10.0"]
    generators = ["CMakeToolchain", "cmake_find_package_multi"]

    def package(self):
        self.copy("*.hpp")

    def package_id(self):
        self.info.header_only()
