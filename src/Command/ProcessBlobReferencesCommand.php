<?php

namespace App\Command;

use App\Service\BlobConsistencyService;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputArgument;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Style\SymfonyStyle;

class ProcessBlobReferencesCommand extends Command
{
    protected static $defaultName = 'app:check-blobs';

    private BlobConsistencyService $blobReferenceService;

    public function __construct(BlobConsistencyService $blobReferenceService)
    {
        parent::__construct();
        $this->blobReferenceService = $blobReferenceService;
    }

    protected function configure()
    {
        $this
            ->setDescription('Checks for blob inconsistencies across specified tables.')
            ->addArgument('table', InputArgument::OPTIONAL, 'Table to check (or leave empty to check all tables)');
    }

    protected function execute(InputInterface $input, OutputInterface $output): int
    {
        $io = new SymfonyStyle($input, $output);
        $table = $input->getArgument('table');

        try {
            if ($table) {
                $this->blobReferenceService->processReferencesForTable($table, $io);
            } else {
                $this->blobReferenceService->processAllReferences($io);
            }
            $io->success('Blob check completed successfully.');
        } catch (\Exception $e) {
            $io->error('Error during blob check: ' . $e->getMessage());
            return Command::FAILURE;
        }

        return Command::SUCCESS;
    }
}
