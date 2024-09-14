<?php

namespace App\Command;

use App\Service\GetReferencesCounts;
use Doctrine\DBAL\Exception;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Input\InputArgument;
use Symfony\Component\Console\Input\InputOption;

class CheckBlobConsistencyCommand extends Command
{
    protected static $defaultName = 'app:check-blob-consistency';

    private GetReferencesCounts $getReferencesCounts;

    public function __construct(GetReferencesCounts $getReferencesCounts)
    {
        parent::__construct();
        $this->getReferencesCounts = $getReferencesCounts;
    }

    protected function configure(): void
    {
        $this
            ->setDescription('Check the consistency of blob references in the database.')
            ->addArgument('batchSize', InputArgument::OPTIONAL, 'The batch size for processing records', 1000)
            ->addOption('more-info', null, InputOption::VALUE_NONE, 'If set, the output will include detailed information');
    }

    /**
     * @throws Exception
     */
    protected function execute(InputInterface $input, OutputInterface $output): int
    {
        $batchSize = (int) $input->getArgument('batchSize');
        $verbose = $input->getOption('more-info');

        $output->writeln('Starting blob consistency check...');

        // Fetch references from the database
        $numReferences = $this->getReferencesCounts->getNumReferences();
        $actualNumReferences = $numReferences['actualNumReferences'];
        $expectedNumReferences = $numReferences['numReferences'];

        // Compare actual references with expected references
        $inconsistencies = $this->findInconsistencies($actualNumReferences, $expectedNumReferences);

        // Output results
        if ($verbose) {
            $output->writeln('Detailed results:');
            foreach ($inconsistencies as $inconsistency) {
                $output->writeln($inconsistency);
            }
        } else {
            $output->writeln('Summary:');
            $output->writeln(sprintf('Found %d inconsistencies.', count($inconsistencies)));
        }

        return Command::SUCCESS;
    }

    private function findInconsistencies(array $actualNumReferences, array $expectedNumReferences): array
    {
        $inconsistencies = [];

        foreach ($actualNumReferences as $id => $count) {
            if (!isset($expectedNumReferences[$id])) {
                $inconsistencies[] = sprintf('Blob ID %s is missing from the expected references.', $id);
            } elseif ($count !== $expectedNumReferences[$id]) {
                $inconsistencies[] = sprintf('Blob ID %s has a mismatch: actual %d, expected %d.', $id, $count, $expectedNumReferences[$id]);
            }
        }

        foreach ($expectedNumReferences as $id => $count) {
            if (!isset($actualNumReferences[$id])) {
                $inconsistencies[] = sprintf('Blob ID %s is missing from the actual references.', $id);
            }
        }

        return $inconsistencies;
    }
}
