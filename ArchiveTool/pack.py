import glob
from sys import argv
import zlib
import struct
from pathlib import Path

print("Archive Packer version 0.1")

magic = 0xD1AB011CA1
version = 1
path_limit = 260
if len(argv) != 3:
    exit()

file_list = glob.glob(argv[1], recursive=True)
file_list = [x.replace("\\", "/") for x in file_list if x.find('.')
             != -1 and Path(x).stat().st_size != 0]
number_of_files = len(file_list)


header = struct.pack("QQQ", magic, version, number_of_files)
file_descriptors = []
current_size = 0


class Descriptor():
    def __init__(self, name, uncompressed_size, size, data):
        self.name = name
        self.offset = len(header) + ((path_limit + 36) * number_of_files)
        self.offset += current_size
        self.size = size
        self.uncompressed_size = uncompressed_size
        self.is_compressed = 1
        self.data = data


print(f"Packing {number_of_files} files into archive\n")
compress_ratio = 0
for file in file_list:
    with open(file, "rb") as f:
        print(f"Adding and Compressing file {file}...")
        data = f.read()
        data_uncompressed_size = len(data)
        data = zlib.compress(data, 9)
        data_size = len(data)
        fdesc = Descriptor(
            bytearray(file, "ASCII"), data_uncompressed_size, data_size, data)
        if data_size == 0 or data_uncompressed_size == 0:
            print(f"error {file} empty!")
            exit()
        file_descriptors.append(fdesc)

        compress_ratio += (data_size / data_uncompressed_size) * 100
        f.close()
        current_size += data_size

descriptor_data = bytearray()
archive_data = bytearray()
print(f"\nBuilding archive {argv[2]} ({number_of_files} files)")
for fd in file_descriptors:
    if len(fd.name) > path_limit:
        print(f"error file name too long! - {fd.name}")
        exit()
    descriptor_data.extend(fd.name)
    descriptor_data.extend(b"\x00" * (path_limit - len(fd.name)))
    descriptor_data.extend(b"\x00" * 4)  # padding
    descriptor_data.extend(struct.pack(
        "QQQQ", fd.offset, fd.size, fd.uncompressed_size, fd.is_compressed))
    archive_data.extend(fd.data)

archive = header + descriptor_data + archive_data
with open(argv[2], "wb") as f:
    f.write(archive)
    f.close()
print(f"Done! Archive {argv[2]} is written to file")
print(f"Compression Ratio Average {compress_ratio / number_of_files}%\n")
