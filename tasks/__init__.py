import importlib
import platform

import yaml
from dl_signing_tools.tasks import sign
from dl_val_toolkit.bootstrap import bootstrap
from dl_val_toolkit.build import build
from dl_val_toolkit.distclean import distclean
from dl_val_toolkit.package import package
from dl_val_toolkit.test import test
from dl_val_toolkit.upload import upload
from dl_val_toolkit.upload_dependencies import upload_dependencies
from invoke import Collection, task
from invoke.tasks import Task


def _conan_data():
    with open('conandata.yml', 'r', encoding='utf-8') as f:
        return yaml.safe_load(f)


@task
def make_installers(ctx, sign_files='False', config=None):
    """Create platform installers after a build."""
    build_config = config or 'Release'
    if platform.system() == 'Windows':
        print('Windows installer generation is not implemented for PDFRotate.')
        return

    ctx.run('rm -rf Installers')
    cmdfmt = './MakeLinuxInstaller.sh {version} {config}'
    ref = cmdfmt.format(version=_conan_data()['CLU_PDF_Rotate']['version'], config=build_config)
    ctx.run(ref)


# Make a top level task list for this module
tasks = [v for v in locals().values() if isinstance(v, Task)]

# And include at the top level, all the build and check tasks from
# dl_val_toolkit
tasks.extend([bootstrap,
              build,
              distclean,
              package,
              test,
              upload,
              upload_dependencies,
              sign])

# Load the Jenkins tasks form dl_pre_commit_hooks
try:
    jenkins_tasks = importlib.import_module('dl_pre_commit_hooks.tasks.jenkins')
    tasks.append(jenkins_tasks)
except ModuleNotFoundError:
    pass

# Bring in the conan tasks from the module of that name, plus all the top
# level tasks
ns = Collection(*tasks)

# Echo things by default
ns.configure({'run': {'echo': 'true'}})
