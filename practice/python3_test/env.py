import os

env = os.environ
print("hi")
print(env)
for i in env:
	print(i)
	print(env.get(i))
	print()