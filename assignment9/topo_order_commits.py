import os
import sys
import zlib

def topo_order_commits():
    commits_dict = dict()
    branches_dict = dict()
    root_commits = []
    commits_sorted = []

    build_commits(commits_dict)
    build_branches(branches_dict)

    for c in commits_dict:
        if not commits_dict[c].parents:
            root_commits.append(c)

    build_commits_sorted(commits_dict, sorted(root_commits), commits_sorted)

    for i, c in enumerate(commits_sorted):
        if i > 0 and commits_sorted[i-1] not in commits_dict[c].children:
            print('=', end='')
            print(*list(commits_dict[c].children))
        if c in branches_dict:
            print(c, end=' ')
            print(*sorted(branches_dict[c]), sep=' ')
        else:
            print(c)
        if i+1 < len(commits_sorted) and commits_sorted[i+1] not in commits_dict[c].parents:
            print(*list(commits_dict[c].parents), sep=' ', end='')
            print('=\n')
    return 0


def build_commits_sorted(commits_dict, root_commits, commits_sorted=[]):
    visited = set()
    while root_commits:
        node = commits_dict[root_commits[-1]]
        added = False
        for c in node.children:
            if c not in visited:
                root_commits.append(c)
                added = True
                break
        if not added:
            root_commits.pop()
            commits_sorted.append(node.commit_hash)
            visited.add(node.commit_hash)

def build_commits(commits_dict=[]):
    objects = os.path.realpath(os.path.join(get_git_dir(), "objects"))
    commit_files = []
    for root, _, files in os.walk(objects):
        for f in files:
            path = os.path.join(root, f)
            data = zlib.decompress(open(path, 'rb').read())
            if data.startswith(b'commit'):
                # found commit object
                if path.startswith(objects):
                    # remove path of objects from full path
                    path = path[len(objects)+1:]
                # remove / and \ (Windows) in path
                commit = path.replace('/', '').replace('\\', '')
                decoded = data.decode()
                parent = ""
                for line in decoded.split("\n"):
                    if line.startswith("parent "):
                        parent = line[7:]
                if commit not in commits_dict:
                    commits_dict[commit] = CommitNode(commit)
                if parent:
                    if parent not in commits_dict:
                        commits_dict[parent] = CommitNode(parent)
                    commits_dict[commit].parents.add(parent)
                    commits_dict[parent].children.add(commit)

def build_branches(branches_dict=dict()):
    # look for local branches in .git/refs/heads
    heads = os.path.join(os.path.join(get_git_dir(), "refs"), "heads")
    branches_list = []
    for root, _, files in os.walk(heads):
        for f in files:
            path = os.path.join(root, f)
            if path.startswith(heads):
                # remove path of heads from full path
                path = path[len(heads)+1:]
            branches_list.append(path)
    for b in branches_list:
        hash = open(os.path.join(heads, b), 'r').read().replace('\n', '')
        if hash not in branches_dict:
            branches_dict[hash] = []
        branches_dict[hash].append(b)

# find .git, look in parent directory if not found until file system root
def get_git_dir():
    # get absolute current path
    path = os.path.abspath(os.curdir)
    while True:
        for d in os.listdir(path):
            if (d == ".git"):
                # found .git folder, use os.path.realpath get rid of extra "./"" and "../"
                return os.path.realpath(os.path.join(path, d))
        if os.path.dirname(path) == path:
            # dirname = path at root, i.e. we are at root and still have not found .git
            break
        path = os.path.join(path, os.pardir)
    # could not find .git
    sys.exit("Not inside a Git repository")


class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()

    def __str__(self):
        return f'commit_hash: {self.commit_hash}, parents: {self.parents}, children: {self.children}'

    def __repr__(self):
        return f'CommitNode<commit_hash: {self.commit_hash}, parents: {self.parents}, children: {self.children}>'


if __name__ == '__main__':
    topo_order_commits()
