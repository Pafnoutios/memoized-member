import os

from conans import ConanFile, tools


class MemoizedmemberConan(ConanFile):
    name = "memoized_member"
    version = "1.2"
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of Memoizedmember here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")
    exports_sources = "include/*"
    no_copy_source = True
    # No settings/options are necessary, this is header only

    def package(self):
        self.copy("*.hpp")
