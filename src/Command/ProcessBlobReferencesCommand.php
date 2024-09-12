<?php

namespace App\Command;

use App\Service\BlobReferenceService;
use Symfony\Component\Console\Command\Command;
use Symfony\Component\Console\Input\InputInterface;
use Symfony\Component\Console\Output\OutputInterface;
use Symfony\Component\Console\Style\SymfonyStyle;

class ProcessBlobReferencesCommand extends Command
{
    private BlobReferenceService $blobReferenceService;

    public function __construct(BlobReferenceService $blobReferenceService)
    {
        parent::__construct();
        $this->blobReferenceService = $blobReferenceService;
    }

    protected function configure(): void
    {
        $this->setDescription('Process blob references in batches to detect inconsistencies.');
        $this->setName('app:process-blob-references');
    }

    protected function execute(InputInterface $input, OutputInterface $output): int
    {
        $io = new SymfonyStyle($input, $output);

        $io->title('Starting blob reference consistency check...');

        // Start the processing
        $this->blobReferenceService->processReferences($io);

        $io->success('Finished processing blob references.');
        return Command::SUCCESS;
    }
}
