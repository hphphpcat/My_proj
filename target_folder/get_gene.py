x=1
i=1
while i<=128:
    i+=i
    x+=x
print(x)

with open("bb/minigraph/test/MT-human.fa", "r") as file:
    gene_fragments = file.readlines()

# 使用join方法将基因片段拼接在一起并去掉换行符
full_gene_sequence = ''.join(gene_fragments).replace('\n', '')
print(full_gene_sequence)

print("----")
with open("bb/minigraph/test/MT-chimp.fa", "r") as file:
    gene_fragments = file.readlines()

# 使用join方法将基因片段拼接在一起并去掉换行符
full_gene_sequence = ''.join(gene_fragments).replace('\n', '')
print(full_gene_sequence)
