<?php
namespace App;

use Symfony\Bundle\FrameworkBundle\Kernel\MicroKernelTrait;
use Symfony\Component\HttpKernel\Kernel as BaseKernel;
use Symfony\Component\DependencyInjection\Compiler\PassConfig;
use Symfony\Component\DependencyInjection\ContainerBuilder;

class Kernel extends BaseKernel
{
    use MicroKernelTrait;

    protected function build(ContainerBuilder $container)
    {
        $container->addCompilerPass(new class implements \Symfony\Component\DependencyInjection\Compiler\CompilerPassInterface {
            public function process(ContainerBuilder $container)
            {
                foreach ($container->findTaggedServiceIds('console.command') as $id => $tags) {
                    echo "Found command: $id\n";
                }
            }
        }, PassConfig::TYPE_BEFORE_OPTIMIZATION, -1000);
    }
}
