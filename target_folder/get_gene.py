import random
# # print(len("GATCACAGGTCTATCACCCTATTAACCACTCACGGGAGCTCTCCATGCATTTGGTATTTTCGTCTGGGGG"))
# # DNA碱基
dna_bases = "ATCG"

for i in range(20):
    print(str(i)+'.'+'fa' ,end=' ')
#  生成1e5个随机的DNA序列
import random
sequence_length = 2000 # 每个序列的长度

sequences = [[] for i in range(20)]
for i in range(20):
        random_sequence = ''.join(random.choice(dna_bases) for _ in range(sequence_length))
        for j in random_sequence:
            sequences[i].append(j)


for i in range(1,6):
    sequences[i] = list(sequences[0])
    for j in range(5,100):
        sequences[i][j] = random.choice(dna_bases)
    for j in range(450,600):
        sequences[i][j] = random.choice(dna_bases)
    # for j in range(4050,4505):
    #     sequences[i][j] = random.choice(dna_bases)
# 将DNA序列写入fa文件
name = '>HS#NC_012920.'

for i in range(20):
    fn = '/home/yqmao/data/My_proj/target_folder/minigraph1/'+str(i)+".fa"
    with open(fn, "w") as file:
        file.write(name + str(i) +  "\n")
        for i, sequence in enumerate(sequences[i], start=1):
            file.write(sequence )
            
            

